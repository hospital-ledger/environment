# Quickly Install and Setup Scripts (QISS)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The branch Version-1.0 and Version-2.0 are abandoned, this master branch only support the install for Version-3.0 and higher Version. It is my first time to build a big project, I'm sincerely hope that you can help me to improve the level of coding and designing.

## Features

Version-3.0: It provides a method for users to install the network on CentOS 7 computer with X86_64 CPU which with 40G Disk free and more than 2G Memory free. The operating steps are on the Readme file in the branch.

Version-4.0: It provides a method for users to install the network on Ubuntu 18.04.3 LTS Version and CentOS 7 computer with X86_64 CPU which with more than 40G Disk free and more than 2G Memory free. You can run them quickly by the installer in master since this version.

Installer: It provides a method for users to install the network quickly, you can follow the steps under. Now you can install with the installer, you can change the Version in the "installer.sh", the latest Version is recommended, you also can edit it with the element in the support list. You can get the guide in "installer.sh".

## Support List

| Date            | Branch          | Version | States                | Description                   |
| --------------- | --------------- | ------- | --------------------- | ----------------------------- |
| 2020-01-18 Tue. | Version-1.0/2.0 | 1.0/2.0 | Abandoned             | First Scripts for installing. |
| 2020-02-08 Sat. | Version-3.0     | 3.0     | Active(Not Recommend) | CentOS 7(Time consuming).     |
| 2020-02-22 Sat. | Version-4.0     | 4.0     | Active(Stable)        | Add Ubuntu and tidy codes.    |
| 2020-03-03 Tue. | Version-4.1     | 4.1     | Active                | Improve the log for error.    |
| Later           | Version-4.2     | 4.2     | Planning              | Sort the log.                 |
| Later           | Version-4.3     | 4.3     | Planning              | Add remove.sh.                |

## Installing Steps

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Firstly, you need to clone the scripts on your device. You need to install git or download the scripts as zip files on the GitHub.com, for installing the git and clone the scripts you can do like that under. 

```Shell
# For CentOS 7
yum install git

# For Ubuntu 18.04.3 LTS
sudo apt install git

# After install the git, you can clone the repostory.
git clone https://github.com/hospital-ledger/scripts.git
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Secondly, you can open the directory for the scripts, run the "installer.sh" to start the installation process. Before that, you need to log in the system with root account.

```Shell
# Switch account to root
su
# And then you need to input your key of the root account.

cd /scripts
./installer.sh
# If there exists some problem to run the command, you may need to do that firstly.
chmod +x ./installer.sh
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Finally, you need to check the software packages and the network, make sure they are working correctly.

## Software Packages

| Name           | Version          | Command           | Remarks              |
| -------------- | ---------------- | ----------------- | -------------------- |
| Go             | 1.13.4 or higher | go version        | None                 |
| Node.js        | v8.9.4           | node -v           | It must be "v8.9.4". |
| npm            | 5.6.0            | npm -v            | It must be "5.6.0"   |
| Docker         | Latest           | docker -v         | None                 |
| Docker-compose | Latest           | docker-compose -v | None                 |
| GCC            | 7.3.0 or higher  | gcc -v            | None                 |
| Fabric         | 2.0.0            | None              | None                 |

## Docker Images

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;You can check the images with the command under.

```Sehll
docker images
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;If there lack some images, for example the hyperledger/fabric-tools version 2.0.0 and latest, you can do like the command under.

```Shell
docker pull hyperledger/fabric-tools:2.0.0
docker pull hyperledger/fabric-tools:latest
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;If the network is slow, you can run the "dockerDeamon.sh".

| Name                         | Version           | Size   |
| ---------------------------- | ----------------- | ------ |
| hyperledger/fabric-tools     | 2.0.0             | 514M   |
| hyperledger/fabric-peer      | 2.0.0             | 57.2M  |
| hyperledger/fabric-orderer   | 2.0.0             | 39.7M  |
| hyperledger/fabric-ccenv     | 2.0.0             | 529M   |
| hyperledger/fabric-baseos    | 2.0.0             | 6.9M   |
| hyperledger/fabric-javaenv   | 2.0.0             | 507M   |
| hyperledger/fabric-nodeenv   | 2.0.0             | 275M   |
| hyperledger/fabric-ca        | 1.4.4             | 150M   |
| hyperledger/fabric-zookeeper | 0.4.18            | 276M   |
| hyperledger/fabric-kafka     | 0.4.18            | 270M   |
| hyperledger/fabric-couchdb   | 0.4.18            | 261M   |
| hyperledger/fabric-tools     | 2.0               | 514M   |
| hyperledger/fabric-peer      | 2.0               | 57.2M  |
| hyperledger/fabric-orderer   | 2.0               | 39.7M  |
| hyperledger/fabric-ccenv     | 2.0               | 529M   |
| hyperledger/fabric-baseos    | 2.0               | 6.9M   |
| hyperledger/fabric-javaenv   | 2.0               | 507M   |
| hyperledger/fabric-nodeenv   | 2.0               | 274M   |
| hyperledger/fabric-ca        | 1.4               | 150M   |
| hyperledger/fabric-zookeeper | latest            | latest |
| hyperledger/fabric-kafa      | latest            | latest |
| hyperledger/fabric-couchdb   | latest            | latest |
| hyperledger/fabric-baseos    | latest            | latest |
| hyperledger/fabric-peer      | latest            | latest |
| hyperledger/fabric-orderer   | latest            | latest |
| hyperledger/fabric-ccenv     | latest            | latest |
| hyperledger/fabric-tools     | latest            | latest |
| hyperledger/fabric-ca        | latest            | latest |
| hyperledger/fabric-javaenv   | latest            | latest |
| hyperledger/fabric-nodeenv   | latest            | latest |
| alpine                       | 3.10              | 5.56M  |
| golang                       | 1.13.4-alpine     | 359M   |
| golang                       | 1.13.4-alpine3.10 | 359M   |

## Log Information

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The installer provides log for error and operation information, the path is "/opt/install-log", you can delete the log quickly by running the "removeLog.sh".

</hr>

Nankai University 2020 &copy; All Rights Reserved.

```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
```

