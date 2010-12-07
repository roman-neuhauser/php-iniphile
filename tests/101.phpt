--TEST--
from_file() with nonexistent path throws iniphile_error
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

try {
    $ini = iniphile::from_file(dirname(__FILE__) . "/nonexistent");
} catch (iniphile_error $e) {
    echo "ok";
}

?>
--EXPECT--	
ok
