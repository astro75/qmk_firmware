# TODO
- https://docs.splitkb.com/hc/en-us/articles/360014402359-How-can-I-make-RGB-animations-prettier-on-the-Kyria-

# Practice
https://www.keybr.com/
https://monkeytype.com/

# Keyboard layout
## EurKey
Although the EurKey [website](https://eurkey.steffen.bruentjen.eu/layout.html) states that `Shift+AltGr+m` is the [compose key](https://en.wikipedia.org/wiki/Compose_key) and the [download](https://eurkey.steffen.bruentjen.eu/download.html) it provides for v1.2 has support for it, the version in most (all?) Linux distributions does not have it.

Up to `xkeyboard-config-2.25` it was included, but [this patch](https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config/-/issues/95) (which was supposed to update the layout to v1.2) removed the Multi key while adding the Greek dead key.

See https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config/-/issues/246
## 

# Unicode
Tap C-S-u, tap unicode code point, tap space

## Works
- Terminal (kitty, Gnome Terminal, Konsole)
- Firefox
- Discord
- LibreOffice

## Partial
- Evolution
  Works for compose, but shortcut for other fields
- KeePassXC
  Does NOT work for passphrase field

## Doesn't work
- Sublime Text

ibus also has support for emoji via C-S-e, but support is quite rare
## Works
- kitty

# Doesn't work
- Firefox
- Discord
probably lots more


Can always use the "Characters" app

# Part vendor finding
https://www.oemsecrets.com/compare/1/EUR/STM32F303CCT6