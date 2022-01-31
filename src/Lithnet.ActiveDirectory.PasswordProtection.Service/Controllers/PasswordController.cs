using Lithnet.ActiveDirectory.PasswordProtection.Server;
using Lithnet.ActiveDirectory.PasswordProtection.Server.Providers;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Lithnet.ActiveDirectory.PasswordProtection.Service.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class PasswordController : ControllerBase
    {
        private readonly ILogger<PasswordController> logger;

        private readonly IPolicyProvider policyProvider;

        private readonly IPasswordValidationProvider passwordValidationProvider;

        public PasswordController(ILogger<PasswordController> logger, IPolicyProvider policyProvider, IPasswordValidationProvider passwordValidationProvider)
        {
            this.logger = logger;
            this.policyProvider = policyProvider;
            this.passwordValidationProvider = passwordValidationProvider;
        }

        [HttpPost]
        public IActionResult Validate(PasswordValidationRequest request)
        {
            var result = this.passwordValidationProvider.ValidatePassword(request);

            if (result.Result == PasswordTestResult.Approved)
            {
                return Ok(result);
            }
            else
            {
                return StatusCode(403, result);
            }
        }
    }
}
