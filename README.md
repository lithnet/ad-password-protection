![](https://lithnet.github.io/images/lppad.png)

Lithnet Password Protection for Active Directory (LPPAD) enhances the options available to an organization wanting to ensure that all their Active Directory accounts have strong passwords.

We're firm believers that good password hygeine is essential in every organization, and it should not be a premium that you pay extra for.

## Features
* Evaluates user password changes using an Active Directory [password filter](https://docs.microsoft.com/en-us/windows/desktop/secmgmt/password-filters) module
* Supports checking passwords against a breached password list
* Supports checking for passwords based on banned words
* Checks common character substitutions (eg P@ssw0rd is normalized to password before being checked against the banned word store)
* Supports custom complexity password rules
  * Minimum password length
  * Length-based complexity rules (allow longer passwords to be less complex)
  * Points-based complexity rules
  * Regular expression matching
* Full PowerShell support
  * Managing the breached password and banned word stores. Add your own banned words and known breached passwords, as well as use popular databases such as the haveibeenpwned downloadable password list
  * Testing passwords and existing hashes against the store
  * Test your user's current passwords in AD against the banned password store
* Passwords never leave the domain controller
* Designed for large environments where high performance is required
* Creates detailed event logs
* Uses a DFS-R friendly data store
* No internet access required
* No additional servers required for deployment
* Group policy support

## System Requirements
#### Password Filter
* Windows Server 2008 R2 or higher
* Microsoft Visual C++ Runtime 14 (2017)
#### PowerShell module
* .NET Framework 4.6
* PowerShell 5
* Microsoft Visual C++ Runtime 14 (2017)

## Getting started
Download the installer from the [releases page](https://github.com/lithnet/ad-password-protection/releases)

Read the [getting started guide](https://github.com/lithnet/ad-password-protection/wiki)

## Keep up to date
* [Visit my blog](http://blog.lithnet.io)
* [Follow me on twitter](https://twitter.com/RyanLNewington)![](http://twitter.com/favicon.ico)

## Acknowledgements
* None of this would be possible without Troy Hunt's [Have I Been Pwned](https://haveibeenpwned.com) service. [Buy him a beer to say thanks](https://haveibeenpwned.com/Donate)! 
* The capability to audit existing user passwords comes from the incredible work Michael Grafnetter has done with his [DSInternals](https://github.com/MichaelGrafnetter/DSInternals) tools.
* Contains code from [Windows-Protocol-Test-Suites](https://github.com/Microsoft/WindowsProtocolTestSuites) used under the [MIT license](https://github.com/Microsoft/WindowsProtocolTestSuites/blob/master/LICENSE.txt) 
