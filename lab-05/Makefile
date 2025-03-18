.PHONY: zip
zip:
	git archive --format zip --output ${USER}-lab06.zip HEAD

.PHONY: html
html:
	pandoc -o README.html -f gfm README.md
	pandoc -o DEBUG.html -f gfm DEBUG.md
