![](https://lithnet.github.io/images/logo-ex-small.png)
# Active Directory Password Filter
The Lithnet Active Directory Password filter enhances the options available to an organization for protecting their accounts in Active Directory with strong passwords.

We're firm believers that good password hygeine is essential in every organization, and it should not be a premium that you pay extra for.

## Features
* No internet access required
* Group policy support
* Supports checked passwords against a breached password list
* Supports checking for banned words in passwords
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

## System Requirements
#### Password Filter
* Windows Server 2008 R2 or higher
#### PowerShell module
* .NET Framework 4.6
* PowerShell 5

## Getting started
Download the installer from the [releases page](https://github.com/lithnet/ad-password-filter/releases)

Read the [getting started guide](https://github.com/lithnet/ad-password-filter/wiki)

## Keep up to date
* [Visit my blog](http://blog.lithiumblue.com)
* [Follow me on twitter](https://twitter.com/RyanLNewington)![](http://twitter.com/favicon.ico)
