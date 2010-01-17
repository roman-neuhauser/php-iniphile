--TEST--
iniphile::get($q, $d) leaves $q, $d unchanged
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c(iniphile $ini, $d)
{
    $q = 'nonexistent';
    $v = $ini->get($q, $d);
    var_dump($q, $d, $v);
    echo "\n";
}

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

c($ini, 'a string');
c($ini, 42);
c($ini, 6.9);
c($ini, true);
c($ini, array(42));

?>
--EXPECT--	
string(11) "nonexistent"
string(8) "a string"
string(8) "a string"

string(11) "nonexistent"
int(42)
int(42)

string(11) "nonexistent"
float(6.9)
float(6.9)

string(11) "nonexistent"
bool(true)
bool(true)

string(11) "nonexistent"
array(1) {
  [0]=>
  int(42)
}
array(1) {
  [0]=>
  string(2) "42"
}
