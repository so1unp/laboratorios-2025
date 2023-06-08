.PHONY: zip
zip:
	git archive --format zip --output ${USER}-lab07.zip HEAD

.PHONY: html
html:
	pandoc -o README.html -f gfm README.md
