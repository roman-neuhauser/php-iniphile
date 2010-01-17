--TEST--
iniphile::get($q, $d) with $q naming a section returns $d
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c($ini, $d)
{
    var_dump($ini->get('strings', $d));
}

$ini = new iniphile(dirname(__FILE__) . "/easy.ini");

c($ini, true);
c($ini, 42);
c($ini, 6.9);
c($ini, 'foo');
c($ini, array('bar'));

?>
--EXPECT--	
bool(true)
int(42)
float(6.9)
string(3) "foo"
array(1) {
  [0]=>
  string(3) "bar"
}
