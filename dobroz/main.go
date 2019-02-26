package main

import (
	"log"

	"github.com/jon/Dobroz/dobroz/actions"
)

func main() {
	app := actions.App()
	if err := app.Serve(); err != nil {
		log.Fatal(err)
	}
}
