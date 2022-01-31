using NUnit.Framework;
using Lithnet.ActiveDirectory.PasswordProtection.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [TestFixture()]
    public class PasswordValidatorBannedSequenceTests
    {
        private List<string> bannedsequences = new List<string>();

        public PasswordValidatorBannedSequenceTests()
        {
            for (int i = 0; i < 1_000_000; i++)
            {
                bannedsequences.Add(Guid.NewGuid().ToString());
            }
        }

        private IPasswordValidator CreateValidator()
        {
            return new PasswordValidatorBannedSequence(Global.LogFactory.CreateLogger("Test"));
        }

        [Test()]
        public void ApproveOnBannedSequenceNotPresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                BannedSequences = bannedsequences
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "afsjahdsklfjakdslfjakldszxcvzxcvzxcvzxcvzcxvzxcvxzfjlkasdjf324523423" };

            Assert.AreEqual(PasswordTestResult.Approved, validator.ValidatePassword(request, policy).Result);
        }

        [Test()]
        public void RejectedBannedSequencePresent()
        {
            var validator = CreateValidator();

            Policy policy = new Policy()
            {
                BannedSequences = new List<string>(){ "abc123"}
            };

            PasswordValidationRequest request = new PasswordValidationRequest { Password = "afsjahdsklfjakdslfjakldszxcvabc123vzcxvzxcvxzfjlkasdjf324523423" };

            Assert.AreEqual(PasswordTestResult.BannedSequence, validator.ValidatePassword(request, policy).Result);
        }
    }
}