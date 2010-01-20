--TEST--
construction with nonexistent path throws iniphile_error
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

try {
    $ini = new iniphile(dirname(__FILE__) . "/nonexistent");
} catch (iniphile_error $e) {
    echo "ok";
}

?>
--EXPECT--	
ok
