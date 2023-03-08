using System;
using System.Management.Automation;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    public abstract class ImportPSCmdlet : PasswordProtectionCmdletBase
    {
        private protected OperationProgress Progress;

        private ProgressRecord overallRecord;
        private ProgressRecord fileReadProgressRecord;
        private ProgressRecord consolidateStoreProgressRecord;
        private ProgressRecord flushStoreProgressRecord;
        private ProgressRecord hibpImportProcessRecord;

        protected void InitializeProgressUpdate(string header)
        {
            this.Progress = new OperationProgress();
            this.overallRecord = new ProgressRecord(1, header, "Starting...");
        }

        protected void EndProgressUpdate()
        {
            if (this.fileReadProgressRecord != null)
            {
                this.fileReadProgressRecord.RecordType = ProgressRecordType.Completed;
                this.WriteProgress(this.fileReadProgressRecord);
                this.fileReadProgressRecord = null;
            }

            if (this.consolidateStoreProgressRecord != null)
            {
                this.consolidateStoreProgressRecord.RecordType = ProgressRecordType.Completed;
                this.WriteProgress(this.consolidateStoreProgressRecord);
                this.consolidateStoreProgressRecord = null;
            }

            if (this.flushStoreProgressRecord != null)
            {
                this.flushStoreProgressRecord.RecordType = ProgressRecordType.Completed;
                this.WriteProgress(this.flushStoreProgressRecord);
                this.flushStoreProgressRecord = null;
            }

            if (this.overallRecord != null)
            {
                this.overallRecord.RecordType = ProgressRecordType.Completed;
                this.WriteProgress(this.overallRecord);
                this.overallRecord = null;
            }

            if (this.hibpImportProcessRecord != null)
            {
                this.hibpImportProcessRecord.RecordType = ProgressRecordType.Completed;
                this.WriteProgress(this.hibpImportProcessRecord);
                this.hibpImportProcessRecord = null;
            }
        }

        protected void WriteProgressUpdate()
        {
            this.overallRecord.StatusDescription = $"Committed {this.Progress.HashesAdded:n0} new hashes to the store and discarded {this.Progress.HashesDiscarded:n0} duplicates";

            this.WriteProgress(this.overallRecord);

            if (this.Progress.FileReadInProgress)
            {
                if (this.fileReadProgressRecord == null)
                {
                    this.fileReadProgressRecord = new ProgressRecord(2, "Importing passwords", "Reading from file");
                    this.fileReadProgressRecord.ParentActivityId = 1;
                }

                this.fileReadProgressRecord.CurrentOperation = $"Read {this.Progress.LinesReadFromFile:n0} lines from file";
                this.fileReadProgressRecord.PercentComplete = Math.Min(100, this.Progress.FileProgressPercent);
                this.fileReadProgressRecord.SecondsRemaining = this.Progress.FileReadSecondsRemaining;

                this.WriteProgress(this.fileReadProgressRecord);
            }
            else
            {
                if (this.fileReadProgressRecord != null)
                {
                    this.fileReadProgressRecord.RecordType = ProgressRecordType.Completed;
                    this.WriteProgress(this.fileReadProgressRecord);
                    this.fileReadProgressRecord = null;
                }
            }

            if (this.Progress.FlushStoreInProgress)
            {
                if (this.flushStoreProgressRecord == null)
                {
                    this.flushStoreProgressRecord = new ProgressRecord(4, "Flushing data", "Writing hashes to disk");
                    this.flushStoreProgressRecord.ParentActivityId = 1;
                }

                this.flushStoreProgressRecord.PercentComplete = Math.Min(100, this.Progress.FlushStorePercent);
                this.flushStoreProgressRecord.SecondsRemaining = this.Progress.FlushStoreSecondsRemaining;

                this.WriteProgress(this.flushStoreProgressRecord);
            }
            else
            {
                if (this.flushStoreProgressRecord != null)
                {
                    this.flushStoreProgressRecord.RecordType = ProgressRecordType.Completed;
                    this.WriteProgress(this.flushStoreProgressRecord);
                    this.flushStoreProgressRecord = null;
                }
            }

            if (this.Progress.HibpReadInProgress)
            {
                if (this.hibpImportProcessRecord == null)
                {
                    this.hibpImportProcessRecord = new ProgressRecord(5, "Downloading hash pages", "Initializing...");
                    this.hibpImportProcessRecord.ParentActivityId = 1;
                }

                this.hibpImportProcessRecord.StatusDescription = $"Page {this.Progress.HibpCurrentHash:n0}/{this.Progress.HibpHashTotal:n0}. {this.Progress.HibpRangesUnchanged} unchanged and {this.Progress.HibpRangesChanged} changed";

                this.hibpImportProcessRecord.PercentComplete = Math.Min(100, this.Progress.HibpProgressPercent);
                this.hibpImportProcessRecord.SecondsRemaining = this.Progress.HibpSecondsRemaining;

                this.WriteProgress(this.hibpImportProcessRecord);
            }
            else
            {
                if (this.hibpImportProcessRecord != null)
                {
                    this.hibpImportProcessRecord.RecordType = ProgressRecordType.Completed;
                    this.WriteProgress(this.hibpImportProcessRecord);
                    this.hibpImportProcessRecord = null;
                }
            }

            if (this.Progress.ConsolidateStoreInProgress)
            {
                if (this.consolidateStoreProgressRecord == null)
                {
                    this.consolidateStoreProgressRecord = new ProgressRecord(3, "Consolidating store", "Sorting and de-duplicating hashes into physical store");
                    this.consolidateStoreProgressRecord.ParentActivityId = 1;
                }

                this.consolidateStoreProgressRecord.PercentComplete = Math.Min(100, this.Progress.ConsolidateStorePercent);
                this.consolidateStoreProgressRecord.SecondsRemaining = this.Progress.ConsolidateStoreSecondsRemaining;

                this.WriteProgress(this.consolidateStoreProgressRecord);
            }
            else
            {
                if (this.consolidateStoreProgressRecord != null)
                {
                    this.consolidateStoreProgressRecord.RecordType = ProgressRecordType.Completed;
                    this.WriteProgress(this.consolidateStoreProgressRecord);
                    this.consolidateStoreProgressRecord = null;
                }
            }
        }
    }
}
