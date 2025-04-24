package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const (
	maxCells                = 1920
	numZones                = 2
	numShelvesPerZone       = 6
	numSectionsPerShelf     = 4
	numShelvesPerSection    = 4
)

type Item struct {
	Name   string
	Amount int
}

type Warehouse struct {
	cells map[string][]Item // Контейнер ячеек и содержащихся в них товаров
}

func NewWarehouse() *Warehouse {
	return &Warehouse{cells: make(map[string][]Item)}
}

func (w *Warehouse) Add(itemName string, amount int, address string) {
	items := w.cells[address]
	summ := 0

	for _, item := range items {
		summ += item.Amount
	}

	for i, item := range items {
		if item.Name == itemName {
			if summ+amount <= 10 {
				items[i].Amount += amount
				w.cells[address] = items
				return
			} else {
				fmt.Printf("Невозможно добавить товар %s в ячейку %s. Превышен лимит в %d единиц.\n", itemName, address, 10)
				return
			}
		}
	}

	if summ+amount <= 10 {
		w.cells[address] = append(items, Item{Name: itemName, Amount: amount})
	} else {
		fmt.Printf("Ячейка %s заполнена. Невозможно добавить товар %s.\n", address, itemName)
	}
}

func (w *Warehouse) Remove(itemName string, amount int, address string) {
	items := w.cells[address]

	for i := range items {
		if items[i].Name == itemName {
			if items[i].Amount >= amount {
				items[i].Amount -= amount
				if items[i].Amount == 0 {
					w.cells[address] = append(items[:i], items[i+1:]...) // Удаляем товар из ячейки
				} else {
					w.cells[address] = items // Обновляем ячейку с оставшимися товарами
				}
				return
			} else {
				fmt.Printf("В ячейке %s недостаточно товара %s.\n", address, itemName)
				return
			}
		}
	}
	fmt.Printf("В ячейке %s нет товара %s.\n", address, itemName)
}

func (w *Warehouse) Info() {
	totalItems := 0

	for _, items := range w.cells {
		for _, item := range items {
			totalItems += item.Amount
		}
	}

	totalCells := numZones * numShelvesPerZone * numSectionsPerShelf * numShelvesPerSection

	fmt.Printf("Общая загрузка склада: %.2f%%\n", float64(totalItems)/(float64(totalCells)*10)*100)

	fmt.Println("Содержимое ячеек:")
	for cellAddress, items := range w.cells {
		if len(items) > 0 {
			fmt.Printf("Ячейка %s: ", cellAddress)
			for _, item := range items {
				fmt.Printf("%s (%d), ", item.Name, item.Amount)
			}
			fmt.Println()
		}
	}

	fmt.Print("Пустые ячейки: ")
	for i := 0; i < numZones; i++ {
		for j := 0; j < numShelvesPerZone; j++ {
			for k := 0; k < numSectionsPerShelf; k++ {
				for l := 0; l < numShelvesPerSection; l++ {
					address := fmt.Sprintf("%c%d%d%d", 'A'+i, j+1, k+1, l+1)
					if _, exists := w.cells[address]; !exists || len(w.cells[address]) == 0 {
						fmt.Printf("%s, ", address)
					}
				}
			}
		}
	}
	fmt.Println()
}

func main() {
	var warehouse = NewWarehouse()
	scanner := bufio.NewScanner(os.Stdin)

	for true {
        fmt.Print("Введите команду: ")
        scanner.Scan()
        input := scanner.Text()

        parts := strings.Fields(input)
        if len(parts) == 0 { 
            continue 
        }

        command := parts[0]

        if command == "EXIT" { 
            break 
        } else if command == "ADD" || command == "REMOVE" { 
            if len(parts) != 4 { 
                fmt.Println("Неверный формат команды.") 
                continue 
            }
            itemName := parts[1]
            amount, err := strconv.Atoi(parts[2])
            if err != nil { 
                fmt.Println("Количество должно быть числом.") 
                continue 
            }
            address := parts[3]

            if command == "ADD" { 
                warehouse.Add(itemName, amount, address) 
            } else { 
                warehouse.Remove(itemName, amount, address) 
            }
        } else if command == "INFO" { 
            warehouse.Info() 
        } else { 
            fmt.Println("Неизвестная команда. Попробуйте еще раз.") 
        }
    }
}
