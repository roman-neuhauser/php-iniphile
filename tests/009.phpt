--TEST--
iniphile::get($q, $d) interpretation of data with boolean $d
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c($ini, $q)
{
    var_dump($ini->get($q, false));
}

$ini = new iniphile(dirname(__FILE__) . "/easy.ini");

c($ini, 'strings.first');
c($ini, 'strings.numstart');
echo "\n";
c($ini, 'qstrings.beginswithone');
c($ini, 'qstrings.beginswithzero');
echo "\n";
c($ini, 'numbers.int');
c($ini, 'numbers.float');
echo "\n";
c($ini, 'booleans1.ok');
c($ini, 'booleans1.ko');
echo "\n";
c($ini, 'booleans2.ok');
c($ini, 'booleans2.ko');
echo "\n";
c($ini, 'booleans3.ok');
c($ini, 'booleans3.ko');

?>
--EXPECT--	
bool(false)
bool(false)

bool(false)
bool(false)

bool(false)
bool(false)

bool(true)
bool(false)

bool(true)
bool(false)

bool(true)
bool(false)
