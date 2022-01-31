using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorRegexRejectMatchTests
    {
        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorRegexRejectMatch(Global.LogFactory.CreateLogger("Test"));
        }

        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexReject = new List<string> { }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must reject this password" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnPatternRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexReject = new List<string> { "(.)\\1{5,}" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must reject this password 111111" };

            Assert.AreEqual(PasswordTestResult.MatchedRejectRegex, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidatePassOnPatternRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexReject = new List<string> { "(.)\\1{5,}" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must pass this password" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnSimpleRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexReject = new List<string> { "reject" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must reject this password" };

            Assert.AreEqual(PasswordTestResult.MatchedRejectRegex, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidatePassOnSimpleRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexReject = new List<string> { "reject" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must pass this password" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}