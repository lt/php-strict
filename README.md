php-strict
==========

This is a PHP extension that makes [scalar type hints (v0.5 RFC implementation)](https://wiki.php.net/rfc/scalar_type_hints_v5) strict by default.

With this extension strict mode can be explicitly disabled per-file by placing `declare(strict_types=0);` at the top of a script.

The default mode can be set using the extended syntax `declare(strict_types = 0 + 'DEFAULT');`, this keeps it compatible with users  not using this extension, due to type coercion

### Prerequisites:

A build of PHP incorporating the changes from the [scalar type hints branch](https://github.com/ircmaxell/php-src/tree/scalar_type_hints_v5).

### How to install:

```
git clone https://github.com/lt/php-strict
cd php-strict
phpize
./configure
make
sudo make install
```
Finally add `extension=strict.so` to your /etc/php.ini

