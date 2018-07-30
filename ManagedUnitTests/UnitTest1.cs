using System;
using System.Diagnostics;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.DirectoryServices.AccountManagement;
using Microsoft.Win32;

namespace ManagedUnitTests
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestDurationPolicyEnabled()
        {
            RegistryKey key = Registry.LocalMachine.CreateSubKey("Software\\Policies\\Lithnet\\PasswordFilter", true);
            key.SetValue("Disabled", 0);

            using (PrincipalContext ctx = new PrincipalContext(ContextType.Domain))
            {
                UserPrincipal p = UserPrincipal.FindByIdentity(ctx, "pwntest");

                if (p == null)
                {
                    Assert.Fail("The test user was not found");
                }

                Stopwatch timer = new Stopwatch();
                timer.Start();
                for (int i = 0; i < 100; i++)
                {
                    try
                    {
                        p.SetPassword(Guid.NewGuid().ToString());
                    }
                    catch (PasswordException e)
                    {
                        Debug.WriteLine("password did not meet requirements");
                    }
                }

                Debug.WriteLine($"Duration: {timer.Elapsed}");
            }
        }

        [TestMethod]
        public void TestDurationPolicyDisabled()
        {
            RegistryKey key = Registry.LocalMachine.CreateSubKey("Software\\Policies\\Lithnet\\PasswordFilter", true);
            key.SetValue("Disabled", 1);

            try
            {
                using (PrincipalContext ctx = new PrincipalContext(ContextType.Domain))
                {
                    UserPrincipal p = UserPrincipal.FindByIdentity(ctx, "pwntest");

                    if (p == null)
                    {
                        Assert.Fail("The test user was not found");
                    }

                    Stopwatch timer = new Stopwatch();
                    timer.Start();
                    for (int i = 0; i < 100; i++)
                    {
                        try
                        {
                            p.SetPassword(Guid.NewGuid().ToString());
                        }
                        catch (PasswordException e)
                        {
                            Debug.WriteLine("password did not meet requirements");
                        }
                    }

                    Debug.WriteLine($"Duration: {timer.Elapsed}");
                }
            }
            finally
            {
                key.SetValue("Disabled", 0);
            }
        }

        [TestMethod]
        public void TestEmptyGuid()
        {
            using (PrincipalContext ctx = new PrincipalContext(ContextType.Machine))
            {
                UserPrincipal p = UserPrincipal.FindByIdentity(ctx, "test");

                if (p == null)
                {
                    Assert.Fail("The test user was not found");
                }

                p.SetPassword(new Guid().ToString());
            }
        }
    }
}
