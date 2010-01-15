--TEST--
iniphile::get() default value pass-through, objects with __toString()
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

class stringy
{
    public function __construct($s)
    {
        $this->s = $s;
    }
    private $s;
    public function __toString()
    {
        return $this->s;
    }
}

$ini = new iniphile(dirname(__FILE__) . "/empty.ini");

var_dump($ini->get("nonexistent", new stringy("contents")));

?>
--EXPECT--	
string(8) "contents"
