--TEST--
Check if sm2 is loaded
--SKIPIF--
<?php
if (!extension_loaded('sm2')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "sm2" is available';
?>
--EXPECT--
The extension "sm2" is available
