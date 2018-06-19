# CoreApp

CoreApp is base application for controling system of containers. It allows to start, stop, install, uninstall, update, freeze, unfreeze and to make snapshot of containers. Input message for the CoreApp comes over IPC (https://github.com/FleX-d/ICL). IPC message contains type of operation, name of container and some kind of messge or small file in text format. CoreApp checks whether the container is aleready stored in a database. The database contains all containers allowed for this system, also contains commands for all operations and checksum of image of a container. CoreApp checks state of requested container. Allowed are four states of containers: Stop, Run, Freeze, Update. All states are linked and it is not possible to change states randomly. Among all states are dependences. For example it is not possible to switch form Run state to Update state directly. It is required to send Stop request at first, then to send Update request. According to request of states the CoreApp reads a corresponding command from database to be executed. After executing command the CoreApp sends acknowledge message over IPC about state of request.

## Example IPC message
Request
  '{"AppID":"NameOfApp","Message":"Nothing or some text","Operation":"Install"}'

Acknowledge
  '{"AppID":"NameOfApp","Message":"Nothing or error message","OperationResult":"False"}'

## An example of procesing is shown in the figure

![example of procesing](Readme/example.png)
