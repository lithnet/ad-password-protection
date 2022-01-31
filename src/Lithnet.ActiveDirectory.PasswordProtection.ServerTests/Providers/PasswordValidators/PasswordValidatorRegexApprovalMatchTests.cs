using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorRegexApprovalMatchTests
    {
        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorRegexApprovalMatch(Global.LogFactory.CreateLogger("Test"));
        }

        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexApprove = new List<string> { }
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
                RegexApprove = new List<string> { "(.)\\1{5,}" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must reject this password" };

            Assert.AreEqual(PasswordTestResult.DidNotMatchApprovalRegex, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidatePassOnPatternRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexApprove = new List<string> { "(.)\\1{5,}" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must pass this password!!!!!!" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnSimpleRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexApprove = new List<string> { "approve" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must reject this password" };

            Assert.AreEqual(PasswordTestResult.DidNotMatchApprovalRegex, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidatePassOnSimpleRegex()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                RegexApprove = new List<string> { "approve" }
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "the test must approve this password" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}