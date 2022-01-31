using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using NLog.Extensions.Logging;
using NUnit.Framework;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server.Tests
{
    [SetUpFixture]
    public class Global
    {
        public static ILoggerFactory LogFactory;

        public static IStore GetStore()
        {
            V3Store store = new V3Store(".\\unit-tests\\");
            return store;
        }

        public static IStore GetCleanStore()
        {
            V3Store store = new V3Store(".\\unit-tests\\");
            return store;
        }

        [OneTimeSetUp]
        [Obsolete]
        public void RunBeforeAnyTests()
        {
            var config = new NLog.Config.LoggingConfiguration();

            // Targets where to log to: File and Console
            var logfile = new NLog.Targets.FileTarget("logfile") { FileName = "unit-test.log" };
            var logconsole = new NLog.Targets.ConsoleTarget("logconsole");
            var logdebugger = new NLog.Targets.DebuggerTarget("logdebugger");

            // Rules for mapping loggers to targets            
            config.AddRule(NLog.LogLevel.Trace, NLog.LogLevel.Fatal, logconsole);
            config.AddRule(NLog.LogLevel.Trace, NLog.LogLevel.Fatal, logfile);
            config.AddRule(NLog.LogLevel.Trace, NLog.LogLevel.Fatal, logdebugger);

            // Apply config           
            NLog.LogManager.Configuration = config;

            var serviceProvider = new ServiceCollection()
                .AddLogging(logger =>
                {
                    logger.SetMinimumLevel(LogLevel.Trace);
                })
                .BuildServiceProvider();

            LogFactory = serviceProvider.GetService<ILoggerFactory>();
            LogFactory.AddNLog();
        }
    }
}
