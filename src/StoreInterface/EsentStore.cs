using System;
using System.Collections.Generic;
using Microsoft.Isam.Esent.Interop;
using System.IO;

namespace StoreInterface
{
    public class EsentStore : Store, IDisposable
    {
        private const string StoreSubPath = "ese\\";
        private const string StoreFileName = "ppwdflt.edb";
        private const string PasswordHashTableName = "pwndpwds";
        private const string PasswordHashTableColumnName = "sha1";

        public static void CreateNewDB(string store)
        {
            JET_INSTANCE instance;
            JET_SESID sesid;
            JET_DBID dbid;
            JET_TABLEID tableid;

            JET_COLUMNDEF columndef = new JET_COLUMNDEF();
            JET_COLUMNID columnid;

            string eseStoreRoot = System.IO.Path.Combine(store, "ese\\");

            Api.JetCreateInstance(out instance, "instance");
            Api.JetSetSystemParameter(instance, JET_SESID.Nil, JET_param.CircularLog, 1, null);
            Api.JetSetSystemParameter(instance, JET_SESID.Nil, JET_param.CreatePathIfNotExist, 1, null);
            Api.JetSetSystemParameter(instance, JET_SESID.Nil, JET_param.LogFilePath, 0, eseStoreRoot);
            Api.JetSetSystemParameter(instance, JET_SESID.Nil, JET_param.SystemPath, 0, eseStoreRoot);
            Api.JetSetSystemParameter(instance, JET_SESID.Nil, JET_param.TempPath, 0, eseStoreRoot);

            Api.JetInit(ref instance);
            Api.JetBeginSession(instance, out sesid, null, null);

            string storepath = System.IO.Path.Combine(store, "ese\\ppwdflt.edb");

            Api.JetCreateDatabase(sesid, storepath, null, out dbid, CreateDatabaseGrbit.OverwriteExisting);

            Api.JetBeginTransaction(sesid);
            Api.JetCreateTable(sesid, dbid, "pwndpwds", 0, 100, out tableid);
            columndef.coltyp = JET_coltyp.Binary;
            columndef.cp = JET_CP.None;
            columndef.cbMax = 20;
            columndef.grbit = ColumndefGrbit.ColumnNotNULL | ColumndefGrbit.ColumnFixed;
            Api.JetAddColumn(sesid, tableid, "sha1", columndef, null, 0, out columnid);
            Api.JetCommitTransaction(sesid, CommitTransactionGrbit.LazyFlush);

            Api.JetBeginTransaction(sesid);
            string indexdef = "+sha1\0\0";

            Api.JetCreateIndex(sesid, tableid,
                "ix_sha1",
                CreateIndexGrbit.IndexDisallowNull | CreateIndexGrbit.IndexPrimary | CreateIndexGrbit.IndexUnique,
                indexdef, indexdef.Length, 100);
            Api.JetCommitTransaction(sesid, CommitTransactionGrbit.None);

            // Terminate ESENT. This performs a clean shutdown.
            Api.JetCloseTable(sesid, tableid);
            Api.JetEndSession(sesid, EndSessionGrbit.None);
            Api.JetTerm(instance);
        }

        private JET_INSTANCE instance;
        private JET_SESID sessionId;
        private JET_DBID dbid;
        private JET_COLUMNID columnid;
        private JET_TABLEID tableid;

        public EsentStore(string storeBasePath)
        : base(20)
        {
            this.OpenDB(storeBasePath);
        }

        private void OpenDB(string storeBasePath)
        {
            string eseStoreRoot = Path.Combine(storeBasePath, EsentStore.StoreSubPath);
            string dbpath = Path.Combine(eseStoreRoot, EsentStore.StoreFileName);

            if (!File.Exists(dbpath))
            {
                throw new FileNotFoundException("The store file could not be found", dbpath);
            }

            Api.JetCreateInstance(out this.instance, "lithnetpwdf");
            Api.JetSetSystemParameter(this.instance, JET_SESID.Nil, JET_param.CircularLog, 1, null);
            Api.JetSetSystemParameter(this.instance, JET_SESID.Nil, JET_param.CreatePathIfNotExist, 1, null);
            Api.JetSetSystemParameter(this.instance, JET_SESID.Nil, JET_param.LogFilePath, 0, eseStoreRoot);
            Api.JetSetSystemParameter(this.instance, JET_SESID.Nil, JET_param.SystemPath, 0, eseStoreRoot);
            Api.JetSetSystemParameter(this.instance, JET_SESID.Nil, JET_param.TempPath, 0, eseStoreRoot);

            Api.JetInit(ref this.instance);
            Api.JetBeginSession(this.instance, out this.sessionId, null, null);

            Api.JetAttachDatabase(this.sessionId, dbpath, AttachDatabaseGrbit.None);

            Api.JetOpenDatabase(this.sessionId, dbpath, null, out this.dbid, OpenDatabaseGrbit.None);
            Api.JetOpenTable(this.sessionId, this.dbid, PasswordHashTableName, null, 0, OpenTableGrbit.None, out this.tableid);

            Api.JetGetTableColumnInfo(this.sessionId, this.tableid, PasswordHashTableColumnName, out JET_COLUMNDEF columnInfo);
            this.columnid = columnInfo.columnid;
        }

        private void CloseDB()
        {
            if (!this.tableid.IsInvalid)
            {
                Api.JetCloseTable(this.sessionId, this.tableid);
            }

            if (this.dbid != JET_DBID.Nil)
            {
                Api.JetCloseDatabase(this.sessionId, this.dbid, CloseDatabaseGrbit.None);
                Api.JetDetachDatabase(this.sessionId, null);
            }

            if (!this.sessionId.IsInvalid)
            {
                Api.JetEndSession(this.sessionId, EndSessionGrbit.None);
            }

            if (!this.instance.IsInvalid)
            {
                Api.JetTerm(this.instance);
            }
        }


        private void ReleaseUnmanagedResources()
        {
            this.CloseDB();
        }

        public void Dispose()
        {
            this.ReleaseUnmanagedResources();
            GC.SuppressFinalize(this);
        }

        ~EsentStore()
        {
            this.ReleaseUnmanagedResources();
        }

        protected override string GetRangeFromHash(string hash)
        {
            return null;
        }

        protected override string GetRangeFromHash(byte[] hash)
        {
            return null;
        }

        public override void ClearStore()
        {
            Api.JetBeginTransaction(this.sessionId);
            try
            {
                if (Api.TryMoveFirst(this.sessionId, this.tableid))
                {
                    Api.JetMove(this.sessionId, this.tableid, JET_Move.First, MoveGrbit.None);

                    do
                    {
                        Api.JetDelete(this.sessionId, this.tableid);
                    } while (Api.TryMoveNext(this.sessionId, this.tableid));

                }

                Api.JetCommitTransaction(this.sessionId, CommitTransactionGrbit.None);
            }
            catch
            {
                Api.JetRollback(this.sessionId, RollbackTransactionGrbit.None);
                throw;
            }
        }

        protected override void AddHashRangeToStore(HashSet<byte[]> hashes, string range, ref int hashesAdded, ref int hashesDiscarded)
        {
            Api.JetBeginTransaction(this.sessionId);

            try
            {
                foreach (byte[] hash in hashes)
                {
                    Api.JetPrepareUpdate(this.sessionId, this.tableid, JET_prep.Insert);

                    try
                    {
                        Api.SetColumn(this.sessionId, this.tableid, this.columnid, hash, SetColumnGrbit.None);
                        Api.JetUpdate(this.sessionId, this.tableid);
                        System.Threading.Interlocked.Increment(ref hashesAdded);
                    }
                    catch (EsentKeyDuplicateException)
                    {
                        Api.JetPrepareUpdate(this.sessionId, this.tableid, JET_prep.Cancel);
                        System.Threading.Interlocked.Increment(ref hashesDiscarded);
                    }
                    catch
                    {
                        Api.JetPrepareUpdate(this.sessionId, this.tableid, JET_prep.Cancel);
                        throw;
                    }
                }

                Api.JetCommitTransaction(this.sessionId, CommitTransactionGrbit.None);
            }
            catch
            {
                Api.JetRollback(this.sessionId, RollbackTransactionGrbit.None);
                throw;
            }
        }

        public override bool IsHashInStore(byte[] hash)
        {
            Api.JetSetCurrentIndex(this.sessionId, this.tableid, null);
            Api.MakeKey(this.sessionId, this.tableid, hash, MakeKeyGrbit.NewKey);
            return Api.TrySeek(this.sessionId, this.tableid, SeekGrbit.SeekEQ);
        }
    }
}