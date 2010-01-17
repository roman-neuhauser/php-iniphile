--TEST--
iniphile::get() default value: objects w/0 __toString() E_ALL|E_STRICT-clean
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

ini_set('error_reporting', E_ALL|E_STRICT);

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

try {
    $ini->get("nonexistent", new stdclass);
} catch (Exception $e) {
    echo "ok";
}

?>
--EXPECT--	
ok
