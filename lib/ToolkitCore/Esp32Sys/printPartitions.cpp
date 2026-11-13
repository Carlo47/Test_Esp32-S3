#include <Arduino.h>
#include <esp_partition.h>

namespace CoreUtils
{
  void printPartitions()
  {
    Serial.println("\n=== Partition Table ===");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    while (it != NULL)
    {
      const esp_partition_t* p = esp_partition_get(it);
      Serial.printf("Label: %-12s Type: 0x%02x SubType: 0x%02x Offset: 0x%06x Size: 0x%06x\n",
                    p->label,
                    p->type,
                    p->subtype,
                    p->address,
                    p->size);
      it = esp_partition_next(it);
    }
    Serial.println("========================\n");
  }
} // namespace CoreUtils