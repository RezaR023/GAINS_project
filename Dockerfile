ARG  CODE_VERSION=latest
#ARG BASE_IMAGE=ubuntu:22.04
#FROM ${BASE_IMAGE}
FROM ubuntu:${CODE_VERSION}
RUN rm /bin/sh && ln -s /bin/bash /bin/sh
LABEL creater="Reza Ramazani sharifabadi, reza_ramazani@ut.ac.ir"

ARG NB_USER="GAINS_USER"
ARG NB_UID="1000"
ARG NB_GID="100"
# Not installing suggested or recommended dependeces which
# are not necessary. So, the size of the final docker image is reduced.
RUN echo 'APT::Install-Suggests "0";' >> /etc/apt/apt.conf.d/00-docker
RUN echo 'APT::Install-Recommends "0";' >> /etc/apt/apt.conf.d/00-docker

USER root
# Prepare conda envorionment and conda installation
ARG conda_version="4.10.3"
# Miniforge installer patch version
ARG miniforge_patch_number="3"
ARG miniforge_python="Mambaforge"
# Miniforge archive to install
ARG miniforge_version="${conda_version}-${miniforge_patch_number}"


# Install all OS dependencies for notebook server that starts but lacks all
# features (e.g., download as all possible file formats)
RUN apt-get update --yes && \
    apt-get install --yes --no-install-recommends \
#    tini \
    wget \
    ca-certificates \
    sudo \
    locales \
    fonts-liberation \
    run-one && \
    apt-get clean && rm -rf /var/lib/apt/lists/* && \
    echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && \
    locale-gen

# Configure environment
ENV CONDA_DIR=/opt/conda \
    SHELL=/bin/bash \
    NB_USER="${NB_USER}" \
    NB_UID=${NB_UID} \
    NB_GID=${NB_GID} \
    LC_ALL=en_US.UTF-8 \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US.UTF-8
ENV PATH="${CONDA_DIR}/bin:${PATH}" \
    HOME="/home/${NB_USER}" \
    CONDA_VERSION="${conda_version}" \
    MINIFORGE_VERSION="${miniforge_version}"

# Copy a script that we will use to correct permissions after running certain commands
COPY fix-permissions /usr/local/bin/fix-permissions
RUN chmod a+rxw /usr/local/bin/fix-permissions

# Create NB_USER with UID=1000 and in the 'users' group
# and make sure these dirs are writable by the `users` group.
RUN echo "auth requisite pam_deny.so" >> /etc/pam.d/su && \
    sed -i.bak -e 's/^%admin/#%admin/' /etc/sudoers && \
    sed -i.bak -e 's/^%sudo/#%sudo/' /etc/sudoers && \
    useradd -l -m -s /bin/bash -N -u "${NB_UID}" "${NB_USER}" && \
    mkdir -p "${CONDA_DIR}" && \
    chown "${NB_USER}:${NB_GID}" "${CONDA_DIR}" && \
    chmod a+rxw /etc/passwd && \
    fix-permissions "${HOME}" && \
    fix-permissions "${CONDA_DIR}"

# Install conda 
WORKDIR /tmp

# Prerequisites installation: conda, mamba, pip, tini
RUN set -x && \
    # Miniforge installer
    miniforge_arch=$(uname -m) && \
    export miniforge_arch && \
    if [ "$miniforge_arch" == "aarm64" ]; then \
      miniforge_arch="arm64"; \
    fi; \
    miniforge_installer="${miniforge_python}-${miniforge_version}-Linux-${miniforge_arch}.sh" && \
    export miniforge_installer && \
    wget --quiet "https://github.com/conda-forge/miniforge/releases/download/${miniforge_version}/${miniforge_installer}" && \
    /bin/bash "${miniforge_installer}" -f -b -p "${CONDA_DIR}" && \
    rm "${miniforge_installer}" && \
    # Conda configuration see https://conda.io/projects/conda/en/latest/configuration.html
    echo "conda ${CONDA_VERSION}" >> "${CONDA_DIR}/conda-meta/pinned" && \
    conda config --system --set auto_update_conda false && \
    conda config --system --set show_channel_urls true && \
    if [[ "${PYTHON_VERSION}" != "default" ]]; then conda install --yes python="${PYTHON_VERSION}"; fi && \
    conda list python | grep '^python ' | tr -s ' ' | cut -d ' ' -f 1,2 >> "${CONDA_DIR}/conda-meta/pinned" && \
    conda install --quiet --yes \
    "conda=${CONDA_VERSION}" \
    'pip' && \
    conda update --all --quiet --yes && \
    conda clean --all -f -y && \
    rm -rf "${HOME}/.cache/yarn" && \
    fix-permissions "${CONDA_DIR}" && \
    fix-permissions "${HOME}"

# Install Tini
RUN conda install --quiet --yes 'tini=0.18.0' && \
 conda list tini | grep tini | tr -s ' ' | cut -d ' ' -f 1,2 >> $CONDA_DIR/conda-meta/pinned && \
 conda clean --all -f -y && \
 fix-permissions "${CONDA_DIR}" && \
 fix-permissions "${HOME}"

USER root

# install pre-requisits 
COPY root-prerequisite /tmp/root-prerequisite
RUN	apt-get update -qq 
RUN apt-get install -y $(cat /tmp/root-prerequisite) \
 && apt-get clean && rm -rf /var/lib/apt/lists/*


# Using a newer CMake
WORKDIR /opt

RUN wget https://github.com/Kitware/CMake/releases/download/v3.23.4/cmake-3.23.4-linux-x86_64.sh -O cmake.sh && chmod +xrw cmake.sh && \
    mkdir /opt/cmake-3.23.4 && ./cmake.sh --skip-license --prefix=/opt/cmake-3.23.4 && \
    rm cmake.sh
ENV PATH="/opt/cmake-3.23.4/bin:${PATH}"


WORKDIR /home/${NB_USER}
USER root

 RUN mkdir G4 && cd G4 && \
     wget https://geant4-data.web.cern.ch/releases/geant4-v11.1.0.tar.gz \
     && tar xzfv geant4-v11.1.0.tar.gz && mkdir G4-build && cd G4-build \
     && cmake -DCMAKE_INSTALL_PREFIX=geant4-v11.1.0-install /home/${NB_USER}/G4/geant4-v11.1.0 \
     && cmake -DGEANT4_INSTALL_DATA=ON . && make -j8 && make install \
     && fix-permissions "${HOME}" &&  fix-permissions "${CONDA_DIR}" \
     && rm -r /home/${NB_USER}/G4/geant4-v11.1.0 

RUN echo ". /home/${NB_USER}/G4/G4-build/geant4-v11.1.0-install/bin/geant4.sh" >> ~/.bashrc

WORKDIR /home/${NB_USER}
USER root

RUN git clone --branch latest-stable --depth=1 https://github.com/root-project/root.git root_src \
    && mkdir builddir installdir && cd builddir \
    && cmake -DCMAKE_INSTALL_PREFIX=/home/${NB_USER}/installdir /home/${NB_USER}/root_src \
    && cmake --build . -j8 --target install  
RUN fix-permissions "${HOME}" && fix-permissions "${CONDA_DIR}" 
RUN  rm -r /home/${NB_USER}/root_src
WORKDIR /home
RUN echo ". ~/installdir/bin/thisroot.sh" >> ~/.bashrc

ENV SIMPATH="/opt/gainsfile" \
    PATH="${SIMPATH}/bin:${PATH}" \
	LD_LIBRARY_PATH="${SIMPATH}/lib:${SIMPATH}/lib/root:${LD_LIBRARY_PATH}" \
	PYTHONPATH="${SIMPATH}/lib/root:${PYTHONPATH}"

# Install Jupyter Notebook, Lab, and Hub
# Generate a notebook server config
# Cleanup temporary files
# Correct permissions
# Do all this in a single RUN command to avoid duplicating all of the
# files across image layers when the permissions change
COPY packages /tmp/packages
RUN conda install --quiet --yes --file /tmp/packages \
 && conda update --all --yes \
 && conda install --quiet --yes scipy \
 && conda clean --all -f -y \
 && npm cache clean --force \
 && jupyter notebook --generate-config \
 && jupyter lab clean \
 && rm -rf "${HOME}/.cache/yarn" \
 && fix-permissions "${CONDA_DIR}" \
 && fix-permissions "${HOME}" 
 
EXPOSE 8888

# copy gains files in docker image
COPY  ./GAINS /home/${NB_USER}/GAINS

# Configure container startup
ENTRYPOINT ["tini", "-g", "--"]
CMD ["start-notebook.sh"]

# Copy local files as late as possible to avoid cache busting
COPY start.sh start-notebook.sh start-singleuser.sh /usr/local/bin/
# Currently need to have both jupyter_notebook_config and jupyter_server_config to support classic and lab
COPY jupyter_notebook_config.py /etc/jupyter/
# Fix permissions on /etc/jupyter as root
USER root
# Prepare upgrade to JupyterLab V3.0 #1205
RUN sed -re "s/c.NotebookApp/c.ServerApp/g" \
    /etc/jupyter/jupyter_notebook_config.py > /etc/jupyter/jupyter_server_config.py \
 && fix-permissions /etc/jupyter/ \
 && jupyter kernelspec install /home/GAINS_USER/installdir/etc/notebook/kernels/root \
 && fix-permissions "${CONDA_DIR}" \
 && fix-permissions "${HOME}"

#Integrating C++ kernels to jupyter
# RUN conda create -n cling && conda install xeus-cling -c conda-forge \
#     && conda install xeus -c conda-forge && conda activate cling

#export LD_LIBRARY_PATH=$ROOTSYS/lib:$PYTHONDIR/lib:$LD_LIBRARY_PATH
#export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH

#Switch back to the user
USER ${NB_UID}
#Set the working directory
WORKDIR "${HOME}"