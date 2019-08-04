using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [System.Serializable]
    public class ApiVersionMismatchException : Exception
    {
        public ApiVersionMismatchException() { }

        public ApiVersionMismatchException(string message) : base(message) { }

        public ApiVersionMismatchException(string message, Exception inner) : base(message, inner) { }

        protected ApiVersionMismatchException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context) : base(info, context) { }
    }
}
