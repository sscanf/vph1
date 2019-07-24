// Before load the commands written below will be executed
// in order to load the application in flash:

// Use the FASTFLASH mode when loading an application
FASTFLASH ONLOAD

// Reset stack pointer 
RESET
// Enable flash module
FLASH
// Select All the flash modules
FLASH SELECT
// Erase all the flash modules
FLASH ERASE
// Unselect the flash modules
FLASH UNSELECT

