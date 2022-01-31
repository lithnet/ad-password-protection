using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class PasswordValidationResponse
    {
        [JsonConverter(typeof(JsonStringEnumConverter))]
        public PasswordTestResult Result { get; set; }

        public object Context { get; set; }

        public string Error { get; set; }
    }
}
