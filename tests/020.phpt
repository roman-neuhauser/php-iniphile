--TEST--
output of quoted input
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c($ini, $q)
{
    var_dump($ini->get($q, ''));
}

$ini = new iniphile(dirname(__FILE__) . "/quirks.ini");

c($ini, 'quotes.one-quoted-string');
c($ini, 'quotes.two-quoted-strings');

?>
--EXPECT--	
string(12) "  foo bar   "
string(15) "" one " "  two""
