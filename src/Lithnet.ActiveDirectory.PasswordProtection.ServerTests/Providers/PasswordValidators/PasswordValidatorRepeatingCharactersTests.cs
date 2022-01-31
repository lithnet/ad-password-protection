using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server.Providers;
using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorRepeatingCharactersTests
    {
        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorRepeatingCharacters(Global.LogFactory.CreateLogger("Test"));
        }

        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MaximumRepeatedCharacters = 0
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "aaaaaaaa" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void FailOnEqualsRepeatLimitStart()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MaximumRepeatedCharacters = 1
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "112345" };

            Assert.AreEqual(PasswordTestResult.RepeatedCharacters, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void FailOnGreaterThanRepeatLimitStart()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MaximumRepeatedCharacters = 1
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "1112345" };

            Assert.AreEqual(PasswordTestResult.RepeatedCharacters, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void FailOnEqualsRepeatLimitEnd()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MaximumRepeatedCharacters = 1
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "123455" };

            Assert.AreEqual(PasswordTestResult.RepeatedCharacters, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void FailOnGreaterThanRepeatLimitEnd()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MaximumRepeatedCharacters = 1
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "1234555" };

            Assert.AreEqual(PasswordTestResult.RepeatedCharacters, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void PassOnLessThanLimit()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                MaximumRepeatedCharacters = 1
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "12345" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}