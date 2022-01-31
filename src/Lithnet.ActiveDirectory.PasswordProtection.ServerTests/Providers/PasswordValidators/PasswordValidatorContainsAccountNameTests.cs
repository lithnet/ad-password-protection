using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorContainsAccountNameTests
    {
        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorContainsAccountName(Global.LogFactory.CreateLogger("Test"));
        }


        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = false
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word john",
                Username = "John"
            };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnFullAccountNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
               PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word john",
                Username = "John"
            };

            Assert.AreEqual(PasswordTestResult.ContainsAccountName, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnUPNAccountNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word john",
                Username = "John@test.com"
            };

            Assert.AreEqual(PasswordTestResult.ContainsAccountName, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnNTAccountNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word john",
                Username = "DOMAIN\\John"
            };

            Assert.AreEqual(PasswordTestResult.ContainsAccountName, validator.ValidatePassword(request, policy).Result);
        }


        [Test()]
        public void ValidateRejectOnPartAccountNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word smithy",
                Username = "Smith"
            };

            Assert.AreEqual(PasswordTestResult.ContainsAccountName, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateApprovedOnWordLessThan3CharsPresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must approve this password even though it contains the word joh",
                Username = "Joh"
            };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateApprovedOnAccountNameNotPresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must approve this password",
                Username = "John"
            };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}