# GAINS + Jupyter Notebook 


GAINS docker image consists of different software packages such as Root and Geant4 as wel as the related libraries in the latest version of ubuntu as a base image.

* fix-permissions: set permissions on a directory after any installation, if a directory needs to be (human) user-writable, run this script on it. It will make everything in the directory owned by the group ${NB_GID} and writable by that group.

* start.sh,start-notebook.sh, start-singleuser.sh: These files used to define different type of starting of the jupyter-notebook.  Here, we use only this file to setup the environment of the running container.

* packages: The list of essential packages together with jupyter notebook and hub. This list is called when using conda install command. If you would like to install different version of the aforementioned packages, you can modify this file(taking into account the possible incompatibility issues).

* jupyter-notebook-config.py: The standard base notebook configuration file.

* root-prerequisite: a list of packeges that need to be installed for the root and geant4. 

## Build the Docker image

To build the image you need to excecute as a root user the Dockerfile and tag it:

```bash
docker build --no-cache -t gains_project .
```
To rebuild the docker image, just exectue the following command:
```bash
docker build -t gains_project .
```

It might take an hour (or more) to install the packges and build the image.

## Run the Docker container
After succefully building the image, you can run a container using the built image:

```bash
docker run -it -p 8888:8888 --name my-gains --rm gains_project
```
To have a shell access in the inside the running container, simply run the following commands:

```bash
docker run -it -d -p 8888:8888 --name my-gains --rm gains_project
```
and then 
```bash
docker exec -it my-gains /bin/bash
```

Since the containers does not have graphical user interface by default, you have to run any software in the batch mode. For example, for running Root-Cern you can use:

```bash
root -b -l
```

To set a password for the jupyter-notebook, you need to add the following line of command in the file called, "jupyter_notebook_config.py":

c.NotebookApp.password = u'sha1:[your hashed password]'

To hide your password, you can use a hashed password using the following website:
"http://www.sha1-online.com/".

Please note that you should rebuild your docker image after editing jupyter_notebook_config.py. 
