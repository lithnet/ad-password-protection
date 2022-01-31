using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server.Providers;
using System;
using System.Collections.Generic;
using System.Text;
using Lithnet.ActiveDirectory.PasswordProtection.Server.Tests;
using Moq;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorLengthTests
    {
        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorLength(Global.LogFactory.CreateLogger("Test"));
        }

        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MinimumLength = 0
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void FailsOnLessThanLength()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MinimumLength = 5
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "1234" };

            Assert.AreEqual(PasswordTestResult.LengthRequirementsNotMet, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void PassOnEqualsMinimumLength()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MinimumLength = 5
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "12345" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void PassOnExceedsMinimumLength()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MinimumLength = 5
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "123456" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}