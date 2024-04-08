# chan_sip

`chan_sip` is the original SIP channel driver from Asterisk that many have grown to love or hate over the years. Replaced by `chan_pjsip` in Asterisk 12, it was deprecated in Asterisk 16, disabled from building by default in Asterisk 19, and was removed from the Asterisk master branch in commit [4095a382da630f8af2aac2d223fcaa70c03b81ba](https://github.com/asterisk/asterisk/commit/4095a382da630f8af2aac2d223fcaa70c03b81ba) on January 3, 2023 (in anticipation of removal for Asterisk 21, to be released in late 2023).

If you are new to Asterisk or getting started with SIP, you should almost certainly use `chan_pjsip`, a newer, more standards-compliant, more feature-rich SIP channel driver. `chan_pjsip` (based on pjproject's PJSIP stack) is actively core supported by Sangoma.

`chan_sip` is maintained here for the benefit of those who still need or prefer it, but you are advised to avoid using it if possible. This version of `chan_sip` is primarily maintained by a community Asterisk developer, and is not official or endorsed by Sangoma in any way. Please do not contact Sangoma or the Asterisk project with issues - any issues should be filed in this repo only.

## Usage

**Manual**

This assumes you are familiar with compiling Asterisk from source. If not, see below.

- Navigate into your Asterisk source directory, either before or after compiling

- Run `wget https://raw.githubusercontent.com/InterLinked1/chan_sip/master/chan_sip_reinclude.sh && chmod +x chan_sip_reinclude.sh && ./chan_sip_reinclude.sh`

- Run `menuselect/menuselect --enable chan_sip menuselect.makeopts` to choose to build `chan_sip`, since `chan_sip` does not build by default.

- Run `make channels && make install` to finish compiling and installing Asterisk with these changes

- Don't forget to `load => chan_sip` in `modules.conf`, since `chan_sip` is disabled by default.

**Automatic**

You can use [PhreakScript](https://github.com/InterLinked1/phreakscript/) to fully compile and install an enhanced version of Asterisk automatically, including with `chan_sip`. Simply specify the `-s` or `--sip` options to the `phreaknet install` command and `chan_sip` will be automatically installed, e.g. run `phreaknet install -s`. Note that if if you install from master or version 21+, it will pull in the enhanced `chan_sip` from this repo. If you install using version 20 (current default) or older, it will simply build the included version of `chan_sip` from those versions (which contain some of the enhancements included below, but not all of them).

Don't forget to `load => chan_sip` in `modules.conf`, since `chan_sip` is disabled by default.

## About This Repo

This is not a fork of Asterisk itself with `chan_sip` still in it. It is, in fact, the opposite: a differential fork of `chan_sip` from current Asterisk so that `chan_sip` can easily continue to be used with modern Asterisk systems. This allows users to upgrade to versions of Asterisk that no longer have `chan_sip` *officially*, but still have it. The goal is to provide those who require `chan_sip` the ability to use it on modern systems.

This version of `chan_sip` was forked from chan_sip immediately prior to the commit removing it. However, it has since diverged and includes a number of improvements that were not merged into Asterisk previously or have been developed since then. These changes are stated below to make it clear what has changed.

The `chan_sip` unit tests are from the Asterisk test suite prior to removal: https://gerrit.asterisk.org/c/testsuite/+/19612

### Changes

- Support for custom parameters (`SIPAddParameter` application; `SIP_PARAMETER` function; `send_oli`, `uri_parameters_instead` sip.conf config options). This functionality is now also available in `chan_pjsip`. This is included in PhreakScript for all versions. Initial patch: https://github.com/InterLinked1/phreakscript/blob/master/patches/sipcustparams.patch and abandoned code review: https://gerrit.asterisk.org/c/asterisk/+/16569

- Support for fax control. This functionality has also been available in `chan_pjsip`. This is included in PhreakScript for all versions. Initial patch: https://github.com/InterLinked1/phreakscript/blob/master/patches/sipfaxcontrol.diff

- Support for Homer (HEP) in `chan_sip`. This functionality has also been available in `chan_pjsip`.

- Support for macros has been removed per the Asterisk deprecation policy

All this said, as both an Asterisk user and developer myself, I recommend you migrate to `chan_pjsip` if you don't have a good reason not to. It will likely serve your needs better. If, however, you still need or prefer `chan_sip`, then this repo is for you.

### Development

I do not actively add functionality to `chan_sip` myself - however, occasionally I do patches for it on request, and I open source these when possible. Any such patches will be incorporated here, in the event they are useful to anyone. All contributions are released under GPLv2 (the same license that Asterisk has).

Improvements or new features are accepted to this repo. However, having learned some things from `chan_sip`'s existence in the official Asterisk tree, please follow these guidelines:

- This repo uses the [Asterisk Coding Guidelines](https://wiki.asterisk.org/wiki/display/AST/Coding+Guidelines), [Asterisk Commit Guidelines](https://wiki.asterisk.org/wiki/display/AST/Commit+Messages), and [Asterisk CHANGES documentation](https://wiki.asterisk.org/wiki/display/AST/CHANGES+and+UPGRADE.txt)

- The directory structure of this repo mirrors the directory structure of the Asterisk repo.

- Minimize changes to existing files to the extent possible, especially `chan_sip.c`, which remains a huge, monolithic source file. New functionality should be added in separate files when possible: both source and header files.

- Write tests when possible using the Asterisk Test Suite.

#### To-Do List

- The usecallmanager patches are not currently incorporated here because they present a huge merge conflict.
