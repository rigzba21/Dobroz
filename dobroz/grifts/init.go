package grifts

import (
	"github.com/gobuffalo/buffalo"
	"github.com/jon/Dobroz/dobroz/actions"
)

func init() {
	buffalo.Grifts(actions.App())
}
