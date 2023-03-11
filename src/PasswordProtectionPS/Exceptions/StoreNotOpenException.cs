using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection.PowerShell
{
    [Serializable]
    public class StoreNotOpenException : Exception
    {
        public StoreNotOpenException() : base()
        {
        }

        public StoreNotOpenException(string message) : base(message)
        {
        }

        public StoreNotOpenException(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected StoreNotOpenException(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context) : base(info, context)
        {
        }
    }
}
