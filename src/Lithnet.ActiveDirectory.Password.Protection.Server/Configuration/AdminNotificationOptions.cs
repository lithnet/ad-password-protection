namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class AdminNotificationOptions
    {
        public string AdminAlertRecipients { get; set; }

        public bool EnableNewVersionAlerts { get; set; }

        public bool EnableCertificateExpiryAlerts { get; set; }
    }
}