using System;

namespace Lithnet.ActiveDirectory.PasswordProtection
{
    [Serializable]
    public class ConfigurationException : LithnetPasswordProtectionException
    {
        public ConfigurationException()
        {
        }

        public ConfigurationException(string message)
            : base(message)
        {
        }

        public ConfigurationException(string message, Exception inner)
            : base(message, inner)
        {
        }

        protected ConfigurationException(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context)
            : base(info, context)
        {
        }
    }
}