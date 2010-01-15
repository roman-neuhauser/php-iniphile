--TEST--
iniphile::get() default type decides return type
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function c(iniphile $ini, $q)
{
    foreach (array('', 0, 0.0, false, array()) as $dflt)
        var_dump($ini->get($q, $dflt));
    echo "\n";
}

$ini = new iniphile(dirname(__FILE__) . "/easy.ini");

c($ini, 'strings.first');
c($ini, 'numbers.int');
c($ini, 'numbers.float');
c($ini, 'booleans1.ok');
c($ini, 'booleans2.ok');
c($ini, 'booleans3.ok');

?>
--EXPECT--	
string(4) "this"
int(0)
float(0)
bool(false)
array(1) {
  [0]=>
  string(4) "this"
}

string(2) "42"
int(42)
float(42)
bool(false)
array(1) {
  [0]=>
  string(2) "42"
}

string(3) "6.9"
int(6)
float(6.9)
bool(false)
array(1) {
  [0]=>
  string(3) "6.9"
}

string(4) "true"
int(0)
float(0)
bool(true)
array(1) {
  [0]=>
  string(4) "true"
}

string(3) "yes"
int(0)
float(0)
bool(true)
array(1) {
  [0]=>
  string(3) "yes"
}

string(1) "1"
int(1)
float(1)
bool(true)
array(1) {
  [0]=>
  string(1) "1"
}
