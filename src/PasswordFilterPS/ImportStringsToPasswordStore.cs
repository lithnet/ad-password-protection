using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management.Automation;

namespace PasswordFilterPS
{
    [Cmdlet(VerbsData.Import, "StringsToPasswordStore")]
    public class ImportStringsToPasswordStore : Cmdlet
    {
        [Parameter(Mandatory = true, Position = 1, ValueFromPipeline = true), ValidateNotNullOrEmpty]
        public string Filename { get; set; }

        [Parameter(Mandatory = false, Position = 2)]

        private StoreInterface.OperationProgress progress;

        protected override void BeginProcessing()
        {
            Global.OpenExistingDefaultOrThrow();
            this.progress = new StoreInterface.OperationProgress();
            base.BeginProcessing();
        }

        protected override void EndProcessing()
        {
            base.EndProcessing();
        }

        protected override void ProcessRecord()
        {
            StoreInterface.Store.ImportPasswordsFromFile(Global.Store, StoreInterface.StoreType.Password, this.Filename, progress: this.progress);
        }
    }
}
