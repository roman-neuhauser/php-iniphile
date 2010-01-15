--TEST--
iniphile::get() default type decides return type
--SKIPIF--
<?php if (!extension_loaded("iniphile")) print "skip"; ?>
--FILE--
<?php

function t($v)
{
    $t = gettype($v);
    return 'array' != $t
        ? $t
        : sprintf(
            '%s<%s>'
          , $t
          , sizeof($v) ? gettype($v[0]) : ''
        )
    ;
}

function c(iniphile $ini, $q)
{
    foreach (array('', 0, 0.0, false, array()) as $dflt) {
        printf(
            "%s -> %s\n"
          , t($dflt)
          , t($ini->get($q, $dflt))
        );
    }
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
string -> string
integer -> integer
double -> double
boolean -> boolean
array<> -> array<string>

string -> string
integer -> integer
double -> double
boolean -> boolean
array<> -> array<string>

string -> string
integer -> integer
double -> double
boolean -> boolean
array<> -> array<string>

string -> string
integer -> integer
double -> double
boolean -> boolean
array<> -> array<string>

string -> string
integer -> integer
double -> double
boolean -> boolean
array<> -> array<string>

string -> string
integer -> integer
double -> double
boolean -> boolean
array<> -> array<string>
