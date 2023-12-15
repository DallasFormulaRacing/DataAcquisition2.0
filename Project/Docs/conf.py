# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
from sphinx.builders.html import StandaloneHTMLBuilder
import subprocess, os

# Doxygen
subprocess.call('doxygen Doxyfile.in', shell=True)


# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Data Acquisition 2.0'
copyright = '2023, Dallas Formula Racing: Embedded Team'
author = 'Dallas Formula Racing: Embedded Team'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    # 'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx.ext.inheritance_diagram',
    'breathe'
    # 'myst_parser'
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

highlight_language = 'c++'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'furo'
html_static_path = ['_static']
# -- Breathe configuration -------------------------------------------------

breathe_projects = {
	"Data Acquisition 2.0": "_build/xml/"
}
breathe_default_project = "Data Acquisition 2.0"
breathe_default_members = ('members', 'undoc-members')




html_logo = "_static/UPDATED-COLORED-LOGO-DFR-2.png"

templates_path = ["_templates"]

# html_sidebars = {
#     '**': [
#         'luv_sphinx.html',
#     ]
# }

# html_sidebars = {
#     '**': [
#         'sidebar/brand.html',
#         'sidebar/search.html',
#         'sidebar/scroll-start.html',
#         'sidebar/navigation.html',
#         'sidebar/ethical-ads.html',

#         "luv_sphinx.html",

#         'sidebar/scroll-end.html'
#     ]
# }



