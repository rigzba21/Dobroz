package actions

import (
  "github.com/gobuffalo/buffalo"
  "github.com/gorilla/mux"
)
// HomeHandler is a default handler to serve up
// a home page.
func HomeHandler(c buffalo.Context) error {
	return c.Render(200, r.HTML("index.html"))
}
