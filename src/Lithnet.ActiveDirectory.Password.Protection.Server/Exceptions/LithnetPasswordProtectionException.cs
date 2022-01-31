using System;
using System.ComponentModel;
using System.Runtime.Serialization;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [Serializable]
    public class LithnetPasswordProtectionException : Exception
    {
        public LithnetPasswordProtectionException()
        {
        }

        public LithnetPasswordProtectionException(string message) : base(message)
        {
        }

        public LithnetPasswordProtectionException(string message, Exception inner) : base(message, inner)
        {
        }


        public LithnetPasswordProtectionException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        }
    }
}