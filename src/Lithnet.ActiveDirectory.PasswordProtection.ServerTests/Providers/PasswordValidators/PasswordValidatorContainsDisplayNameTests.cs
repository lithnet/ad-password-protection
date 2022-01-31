using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorContainsDisplayNameTests
    {
        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorContainsDisplayName(Global.LogFactory.CreateLogger("Test"));
        }

        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfFullName = false
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word john",
                FullName = "John Smith"
            };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnFirstNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
               PreventUseOfFullName = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word john",
                FullName = "John Smith"
            };

            Assert.AreEqual(PasswordTestResult.ContainsFullName, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnLastNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfFullName = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word smith",
                FullName = "John Smith"
            };

            Assert.AreEqual(PasswordTestResult.ContainsFullName, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateRejectOnMiddleNamePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfFullName = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must reject this password because it contains the word andrew",
                FullName = "John Andrew Smith"
            };

            Assert.AreEqual(PasswordTestResult.ContainsFullName, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateApprovedOnWordLessThan3CharsPresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfFullName = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must approve this password even though it contains the word joh",
                FullName = "Joh Smith"
            };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void ValidateApprovedOnFullNameNotPresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                PreventUseOfUsername = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest
            {
                Password = "the test must approve this password",
                Username = "John Smith"
            };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}