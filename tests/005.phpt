--TEST--
iniphile::get() default value pass-through
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c(iniphile $ini, $dflt)
{
    var_dump($ini->get("nonexistent", $dflt));
}

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

c($ini, 0);
c($ini, 0.0);
c($ini, false);
c($ini, "foo");

?>
--EXPECT--	
int(0)
float(0)
bool(false)
string(3) "foo"
