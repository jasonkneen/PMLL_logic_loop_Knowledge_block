import { axios } from "@pipedream/platform"

export async function processBatch(batchFile) {
  const response = await axios({
    method: 'POST',
    url: 'https://api.openai.com/v1/batches',
    headers: {
      'Authorization': `Bearer ${process.env.OPENAI_API_KEY}`,
      'Content-Type': 'application/json'
    },
    data: {
      input_file_id: batchFile,
      endpoint: "/v1/chat/completions",
      completion_window: "24h"
    }
  });
  
  return response.data;
}
