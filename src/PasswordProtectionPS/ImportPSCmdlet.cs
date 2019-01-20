using System;
using System.Collections.Generic;
using System.Linq;
using System.Management.Automation;
using System.Text;
using System.Threading.Tasks;
using Lithnet.ActiveDirectory.PasswordProtection;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    public abstract class ImportPSCmdlet : PSCmdlet
    {
        protected internal OperationProgress Progress;

        private ProgressRecord overallRecord;
        private ProgressRecord fileReadProgressRecord = null;
        private ProgressRecord consolidateStoreProgressRecord = null;
        private ProgressRecord flushStoreProgressRecord = null;

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
        }

        protected void WriteProgressUpdate()
        {
            this.overallRecord.StatusDescription = this.Progress.GetProgressText();

            this.WriteProgress(this.overallRecord);

            if (this.Progress.FileReadInProgress)
            {
                if (this.fileReadProgressRecord == null)
                {
                    this.fileReadProgressRecord = new ProgressRecord(2, "Importing passwords", "Reading from file");
                    this.fileReadProgressRecord.ParentActivityId = 1;
                }

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
