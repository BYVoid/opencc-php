### What is opencc-php?

opencc-php is a PHP wrapper for [Open Chinese Converter](https://github.com/BYVoid/OpenCC)

### Installation

You need to install opencc-dev first, To install OpenCC:

Debian:

    apt-get install libopencc-dev -y

FreeBSD:

    cd /usr/ports/chinese/opencc
    make install clean

To install opencc-php:

    git clone https://github.com/BYVoid/opencc-php.git
    cd pyopencc
    phpize
    ./configure --enable-opencc
    make && make install

### How to use it?

Following is a simple example:

```php
$od = opencc_open("zhs2zht.ini");
$text = opencc_convert($od, "你干什么不干我事。");
echo $text;
opencc_close($od);
```
    
And the output should be:

    你幹什麼不干我事

There are four convertion in opencc:

 * zht2zhs.ini - Traditional Chinese to Simplified Chinese
 * zhs2zht.ini - Simplified Chinese to Traditional Chinese
 * mix2zht.ini - Mixed to Traditional Chinese
 * mix2zhs.ini - Mixed to Simplified Chinese
