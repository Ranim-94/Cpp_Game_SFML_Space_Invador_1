
# VS Code

## Table of Contents

- [VS Code](#vs-code)
  - [Table of Contents](#table-of-contents)
  - [Context](#context)
  - [Version](#version)
  - [Update](#update)

## Context

Documents contains some administrative commands to check 
`VsCode`: version, update,path,...


## Version 

There are 2 commands for the version:

<u> Command 1: </u>

<pre>code --version</pre>

<u> Command 2: </u>

<pre>apt policy code</pre>

<u>Output:</u>

<pre>
Installed: 1.139.1-1790309529
Candidate: 1.139.1-1790309529</pre>

The 2nd command is preferred, since it can show us if thre is an update or not. If there is, the candidate version is different from the installed one

## Update

Refresh `apt` 1st

<pre>sudo apt update </pre>

Then udapte **VsCode** via the following command:

<pre>sudo apt install --only-upgrade code </pre>

