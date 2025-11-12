all:
	docker compose run -w /work/src --rm app make all

run: all
	docker compose run -w /work/src --rm app make run

clean:
	docker compose run -w /work/src --rm app make clean
