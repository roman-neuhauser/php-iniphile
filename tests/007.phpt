--TEST--
iniphile::get() default value: objects w/0 __toString() cause exception, not notice
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

try {
    $ini->get("nonexistent", new stdclass);
} catch (Exception $e) {
    echo "ok";
}

?>
--EXPECT--	
ok
