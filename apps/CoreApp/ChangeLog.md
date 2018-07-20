# Changelog
All notable changes to this project will be documented in this file.

## [Unreleased]
## [0.0.6] - 2018-07-04
### Added
- CoreAppRequestQueue [Martin Strenger]
- All requests are performed in a separate thread

### Changed
- Refactoring [Martin Strenger]
- Fixed memory leakes [Martin Strenger]

### Removed
- None

## [0.0.6] - 2018-07-04
### Added
- None

### Changed
- Refactoring [Martin Strenger]

### Removed
- None

## [0.0.6] - 2018-06-21
### Added
- None

### Changed
- Changes regarding new version of FleXdIPCConnector [Martin Strenger]

### Removed
- None

## [0.0.6] - 2018-06-18
### Added
- None

### Changed
- Edit to using path als last parameter in command
- Edit tool db initDB for create db in /etc/CoreApp
- Edit main for new database path

### Removed
- None

## [0.0.5] - 2018-06-18
### Added
- None

### Changed
- Edit factory, request to save path to install file
- Edit iCoreAppAck for sending error message back and log of runed update

### Removed
- Remove segmeted mesage

## [0.0.5] - 2018-06-08
### Added
- Add new IPCInterface CoreDocIPCInterface [Peter Kocity]
- Add Visitor functionaliti for manager

### Changed
- Edit current IPC functionality for new IPCInterface
- Change appID from string to uint16_t
- Edit Factory metod for uint16_t
- Edit test for factory

### Removed
- Delete old IPCInterface iIPCclient


## [0.0.4] - 2018-04-24
### Added
- Add map of app in manager, create function for access in function [Peter Kocity]
- Add readme
- Add executor function for return text from console

### Changed
- None

### Removed
- None

## [0.0.3] - 2018-04-24
### Added
- Ading class for state of apps and iApp for change state with shared_ptr [Peter Kocity]
- Adding test for this new class

### Changed
- None

### Removed
- None

## [0.0.2] - 2018-04-20
### Added
- Validator of request with template
- Test for validator
- constant string in CoreAppType for operation type and workpath

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-13
### Added
- Lambda functionality for IPCClient
- Manager functionality for tryprocess with labda function
- Manager funtion for return keyWord for database from request type
- Log function for all code

### Changed
- None

### Removed
- None

## [0.0.1] - 2018-04-10
### Added
- Initial version [Peter Kocity]
- Initialization database
- Skeleton of app, reading and writing into database

### Changed
- None

### Removed
- None
