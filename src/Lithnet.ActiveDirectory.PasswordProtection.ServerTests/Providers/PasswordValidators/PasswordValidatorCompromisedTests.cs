using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server;
using System;
using System.Collections.Generic;
using System.Text;
using Moq;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorCompromisedTests
    {
        private IPasswordValidator CreateValidator()
        {
            Mock<IStore> store = new Mock<IStore>();
            store.Setup(t => t.IsInStore("password", StoreType.Password)).Returns(true);

            Mock<IStoreProvider> storeProvider = new Mock<IStoreProvider>();
            storeProvider.SetupGet(t => t.Store).Returns(store.Object);

            return new PasswordValidatorCompromised(Global.LogFactory.CreateLogger("Test"), storeProvider.Object);
        }

        [Test()]
        public void ApprovedOnPolicyDisabled()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                CompromisedPasswordChecking = false
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "password" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void FailsOnBadPassword()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                CompromisedPasswordChecking = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "password" };

            Assert.AreEqual(PasswordTestResult.Compromised, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void PassesOnGoodPassword()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                CompromisedPasswordChecking = true
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "good" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }
    }
}