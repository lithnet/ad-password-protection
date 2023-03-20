![](https://lithnet.github.io/images/lppad.png)

Lithnet Password Protection for Active Directory (LPP) enhances the options available to an organization wanting to ensure that all their Active Directory accounts have strong passwords.

LPP is a module that you install on your Active Directory servers that uses a [password filter](https://docs.microsoft.com/en-us/windows/desktop/secmgmt/password-filters) to inspect passwords as users attempt to change them. Using group policy, you customize the types of checks you want to perform on those passwords and they are either rejected, or approved, and committed to the directory.

LPP gives you the ability to take control of what a good password means to you. Whether you want to adopt the 2018 NIST password recommendations in part, or in full, it provides a rich set of group policy-based controls that allow you to enable any combination of the following checks on attempted password changes.

* **Block compromised passwords from being used**. We've made it super easy to import the HIBP data set, but you can also import any plain-text passwords or NTLM hashes that you can get your hands on.
* **Block passwords based on certain words**. Adding a banned word prevents it from being used as the base of a password. For example, adding the word 'password' to the banned word store, prevents not only the use of that word itself, but common variants such as 'P@ssw0rd', 'pa55word!' and 'password123456!'. LPP is aware of common character substitutions and weak obfuscations and prevents their use through a normalization process. 
* **Define complexity policies based on length**. For example, you can require number, symbol, upper and lower for passwords less than 13 characters, but have no special requirements for passwords 13 characters or longer. Reward length, with less complexity.
* **Regular expression-based policies**. If regular expressions are your thing, you can define a regular expression that the password must match (or not match).
* **Points-based complexity**. Assign points for the use of certain characters and categories and set a minimum point threshold a password must meet.

It also includes the ability to audit your users' existing passwords against the compromised password list. You'll be able to find the weak and known compromised passwords, and force those users to change their password.

## Additional features
* Full PowerShell support which is used to;
  * Synchronize compromise password lists from the haveibeenpwned.com API
  * Add your own banned words and compromised passwords, as well as use popular 3rd party word and password lists
  * Test passwords and existing hashes against the compromised store
  * Check to see if your user's current passwords in AD are found in the compromised password store
* Passwords never leave the domain controller
* Designed for large environments where high performance is required
* Creates detailed event logs
* Uses a DFS-R friendly data store
* No internet access required
* No additional servers required for deployment
* Group policy support

## System Requirements
LPP is only supported on x64 editions of Windows

#### Password Filter
* Windows Server 2012 R2 or higher
* [Microsoft Visual C++ Redistributable package 2015-2022](https://aka.ms/vs/17/release/vc_redist.x64.exe)
#### PowerShell module
* .NET Framework 4.7.2
* Windows PowerShell 5
* [Microsoft Visual C++ Redistributable package 2015-2022](https://aka.ms/vs/17/release/vc_redist.x64.exe)

## Getting started
Download the installer from the [releases page](https://github.com/lithnet/ad-password-protection/releases)

Read the [getting started guide](https://docs.lithnet.io/password-protection/installation/setup-guide) on our documentation site.

## How can I contribute to the project?
* Found an issue and want us to fix it? [Log it](https://github.com/lithnet/ad-password-protection/issues)
* Want to fix an issue yourself or add functionality? Clone the project and submit a pull request

## Enteprise support
Lithnet offer enterprise support plans for our products. Deploy our tools with confidence that you have the backing of the dedicated Lithnet support team if you run into any issues, have questions, or need advice. Reach out to us via our [contact form](https://lithnet.io/products/password-protection#requestaquote) for a quote and more information on this offering.

## Keep up to date
* [Product documentation](http://docs.lithnet.io/password-protection)
* [Visit our blog](http://blog.lithnet.io)
* [Follow us on twitter](https://twitter.com/lithnet_io)![](http://twitter.com/favicon.ico)

## Acknowledgements
* None of this would be possible without Troy Hunt's [Have I Been Pwned](https://haveibeenpwned.com) service. [Buy him a beer to say thanks](https://haveibeenpwned.com/Donate)! 
* The capability to audit existing user passwords comes from the incredible work Michael Grafnetter has done with his [DSInternals](https://github.com/MichaelGrafnetter/DSInternals) tools.
* Contains code from [Windows-Protocol-Test-Suites](https://github.com/Microsoft/WindowsProtocolTestSuites) used under the [MIT license](https://github.com/Microsoft/WindowsProtocolTestSuites/blob/master/LICENSE.txt) 
