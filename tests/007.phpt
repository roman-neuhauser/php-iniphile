--TEST--
iniphile::get() default value: objects w/0 __toString() cause exception
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

error_reporting(error_reporting() & ~E_NOTICE);

try {
    $ini->get("nonexistent", new stdclass);
    echo "bug";
} catch (Exception $e) {
    echo "ok";
}

?>
--EXPECT--	
ok
