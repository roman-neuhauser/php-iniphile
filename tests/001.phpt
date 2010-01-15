--TEST--
construction with nonexistent path throws
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

try {
    $ini = new iniphile(dirname(__FILE__) . "/nonexistent");
} catch (Exception $e) {
    echo "ok";
}

?>
--EXPECTF--	
ok
