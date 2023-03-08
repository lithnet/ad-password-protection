using System;
using System.Management.Automation;
using System.Threading;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    public abstract class PasswordProtectionCmdletBase : PSCmdlet, IDisposable
    {
        private protected CancellationTokenSource token = new CancellationTokenSource();
        private bool disposedValue;

        protected override void StopProcessing()
        {
            this.token.Cancel();
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!this.disposedValue)
            {
                if (disposing)
                {
                    this.token.Dispose();
                }
                this.disposedValue = true;
            }
        }

        public void Dispose()
        {
            this.Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
