--TEST--
iniphile::get($q, $d) returns a copy of $d
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c($ini, $d, $e)
{
    $v = $ini->get('nonexistent', $d);
    if (gettype($d) == 'array')
        $d[] = $e;
    else
        $d = $e;
    var_dump($v);
}

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

c($ini, false, true);
c($ini, 42, 69);
c($ini, 4.2, 6.9);
c($ini, 'foo', 'bar');
c($ini, array('foo'), 'bar');

?>
--EXPECT--	
bool(false)
int(42)
float(4.2)
string(3) "foo"
array(1) {
  [0]=>
  string(3) "foo"
}
