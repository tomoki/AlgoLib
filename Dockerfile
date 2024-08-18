# in your Dockerfile
FROM sphinxdoc/sphinx

WORKDIR /docs
RUN python3 -m pip install sphinx-copybutton

